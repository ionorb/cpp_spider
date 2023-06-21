#!/bin/bash

echo enter starting url:
read url
echo enter curl from the network monitor on your browser \(leave blank if you don\'t know what this is\):
read curl
echo enter the number of pages you want to crawl:
read pages

c++ -Wall -Werror -Wextra -g3 extractlinks.cpp -o spider&& ./spider $url $pages $curl

#I'm not sure what I'm doing wrong. I've tried using the -std=c++11 flag, but that doesn't seem to work either. I'm using g++ 4.8.2 on Ubuntu 14.04. Any help would be appreciated. Thanks!


