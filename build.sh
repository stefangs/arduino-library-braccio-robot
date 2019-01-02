#!/usr/bin/env bash
name='BraccioRobot'
echo $name
mkdir $name
cp -R keywords.txt LICENSE README.md examples library.properties moveRobot.py src $name/
zip -r ${name}.zip $name
rm -r $name
