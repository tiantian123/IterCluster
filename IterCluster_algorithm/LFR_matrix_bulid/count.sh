#!/bin/bash

find . |grep -v git | grep -v bin | grep -v fq | grep -v '/build/' | grep -v 'Third' | grep -v 'test' | grep -v 'py' | xargs wc 
