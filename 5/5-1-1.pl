#!/usr/local/bin/perl 
#  

print "What is your name? "; 
$name = <STDIN>;
chomp($name);
if ($name =~/\bkoyama\b/i) {
    print "Hello, Koyama! How good of you to be here!\n"; 
} elsif ($name =~/\bmori\b/i) {
    print "hello, Mori! and Good Bye!\n"
} else { 
    print "Hello, $name !\n";    # ordinary greeting 
} 
