#!/usr/local/bin/perl 
#  

print "What is your name? "; 
$name = <STDIN>; 
chomp ($name); 
if ($name eq "RYO") { 
    print "Hello, RYO! How good of you to be here!\n"; 
} else { 
    print "Hello, $name!\n";    # ordinary greeting 
} 
