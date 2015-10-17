# MP3Foo
Doing low-level file manipulations to edit MP3s.


In principle a company needed to add some copyright flag on their mp3s, such that it showed 
as being copyrighted on several players (including Winamp, ..). Surprisingly tools to edit 
mp3 headers, although plentiful, didn't allow to set this bit. Well.. if you want to get things 
done you gotta do 'em yourself.

### Usage:

./tool -d DIRECTORY [-v] [-h]

### Compile Options:

gcc *.c -o tool -std=c99


No special lib needed. If you find some bugs.. fix them ;) 

