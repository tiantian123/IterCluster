#ifndef __LFR_UTILS_ARRAYHASH_H__
#define __LFR_UTILS_ARRAYHASH_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <functional>
#include "Define.h"

inline int
ceillog2(unsigned int x) { //fetch from lua source code
  static const unsigned char log_2[256] = {
    0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
  };
  int l = 0;
  x--;
  while (x >= 256) { l += 8; x >>= 8; }
  return l + log_2[x];
}

template<class KEY_TYPE, class VALUE_TYPE>
struct ArrayHash{

    typedef KEY_TYPE KeyType;
    typedef VALUE_TYPE ValueType;

    static const KEY_TYPE INVALID_KEY = -1;

    struct slot {
        KEY_TYPE key;
        VALUE_TYPE value;
        KEY_TYPE next;
        slot() 
            : key(ArrayHash::INVALID_KEY)
            , value(0)
            , next(ArrayHash::INVALID_KEY)
        {}
    };

    ArrayHash(KEY_TYPE block_size)
    {
        int lsize = ceillog2(block_size);
        size = KEY_TYPE(0x1)<<lsize;
        lastfree = size - 1;
        slot_list = new slot[size];
    }
    ArrayHash(){
        int lsize = ceillog2(10000);
        size = KEY_TYPE(0x1)<<lsize;
        lastfree = size - 1;
        slot_list = new slot[size];
    }
    ~ArrayHash()
    {
        if(slot_list)
            delete [] slot_list;
        slot_list = nullptr;
    }

    slot & operator[](KEY_TYPE k)
    {
        slot * s = Query(k);
        if(s != nullptr)
            return *s;
        return *Insert(k,VALUE_TYPE(0));
    }


    void Foreach( void (*f)(slot & item ))
    {
        for(KEY_TYPE i = 0 ; i < size ; i++)
        {
            if(slot_list[i].key != INVALID_KEY)
            {
                f(slot_list[i]);
            }
        }
    }
    /*
    void Foreach( std::function<void(const slot & item)> f )
    {
        for(KEY_TYPE i = 0 ; i < size ; i++)
        {
            if(slot_list[i].key != INVALID_KEY)
            {
                f(slot_list[i]);
            }
        }
    }*/
    KEY_TYPE Clear()
    {
        KEY_TYPE total = 0;
        for(KEY_TYPE i = 0 ; i < size ; i++)
        {
            if(slot_list[i].key != INVALID_KEY)
            {
                total ++ ;
                slot_list[i].key = INVALID_KEY;
                slot_list[i].value = 0;
                slot_list[i].next = INVALID_KEY;
            }
        }
        lastfree = size - 1;
        return total;
    }
    //void trace_hashmap(map*);
    void Del(KEY_TYPE key)
    {
        slot* s = mainposition(key);
        slot* prev = nullptr;
        while(true){
            if (s->key == key) {
                s->key = INVALID_KEY;
                s->value = 0;
                if (prev) {
                    if (prev->next > lastfree) {
                        lastfree = prev->next;
                    }
                    prev->next = s->next;
                    s->next = INVALID_KEY;
                }else { //its head
                    if (s->next >= 0) {
                        if (s->next > lastfree) {
                           lastfree = s->next;
                        }
                        slot* next = &slot_list[s->next];
                        *s = *next;
                        next->key = INVALID_KEY;
                        next->value = 0;
                        next->next = INVALID_KEY;
                    }
                }
                return ;
            }
            if (s->next == INVALID_KEY) {
                return ;
            }
            prev = s;
            s = &slot_list[s->next];
        }
    }

    slot* Insert(KEY_TYPE key , VALUE_TYPE value)
    {
        slot * s = mainposition(key);
        if (s->key == INVALID_KEY) {
            s->key = key;
            s->value = value;
            return s;
        }
        slot* last = mainposition( s->key);
        if (last != s) { //s not in its hash pos, the head of hash link-list which s in is last
            while (last->next != s - slot_list) {
                assert(last->next >= 0);
                last = &slot_list[last->next];
            }
            KEY_TYPE tmp_key = s->key;
            VALUE_TYPE tmp_val = s->value;
            last->next = s->next;
            s->key = key;
            s->value = value;
            s->next = INVALID_KEY;
            Insert(tmp_key, tmp_val);
            return Query(key);
        }
        while (lastfree >0 && lastfree != INVALID_KEY ) {
            slot * temp = &slot_list[lastfree--];
            if (temp->key == INVALID_KEY) {
                temp->key = key;
                temp->value = value;
                temp->next = s->next;
                s->next = (KEY_TYPE)(temp - slot_list);
                return temp;
            }
        }
        rehash();
        return Insert( key, value);
    }

    slot* Query(KEY_TYPE key)
    {
        slot *s = mainposition(key);
        while(true)
        {
            if (s->key == key) {
                return s;
            }
            if (s->next == INVALID_KEY) {
                return nullptr;
            }
            s=&slot_list[s->next];
        }
    }
    public:

    KEY_TYPE size;
    KEY_TYPE lastfree;
    slot* slot_list;

    private:
    inline slot * mainposition( KEY_TYPE key){
        KEY_TYPE hash = key & (size-1);
        return &slot_list[hash];
    }

    void rehash() {
        slot * old_slot = slot_list;
        KEY_TYPE old_size = size;
        size = 2 * old_size;
        lastfree = size - 1;
        slot_list = new slot[size];
        for (KEY_TYPE i=0;i<old_size;i++) {
            slot * s = &old_slot[i];
            if (s->key != INVALID_KEY) {
                Insert(s->key, s->value);
            }
        }
        delete[] old_slot;
    }
} ;

#endif //__LFR_UTILS_ARRAYHASH_H__
