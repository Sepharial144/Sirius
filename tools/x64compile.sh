#!/bin/bash
cmake . -Bbuild -DSIRIUS_TESTS=ON && cmake --build build -j4