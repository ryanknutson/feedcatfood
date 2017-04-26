#!/bin/bash

echo "document.write('\\" > page.js


sed 's/^.*$/&\\/g;' page.html >> page.js


echo "');" >> page.js
