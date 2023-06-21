#!/bin/bash

c++ -Wall -Werror -Wextra -g3 extractlinks.cpp -o spider&& ./spider $1 $2
