#!/usr/local/bin/perl

open(LS,"ls -l |");

$pline=<LS>;

$fbytes=0;
$dbytes=0;
$file=0;
$dir=0;

while($nline=<LS>){
    @elements=split(/\s+/,$nline);
    printf("%3s %2d %5s",$elements[5],$elements[6],$elements[7]);
    if($elements[1]==1){
	printf("%10s","");
	$fbytes+=$elements[4];
	$file++;
    }
    else{
	printf("%10s","<DIR>");
	$dbytes+=$elements[4];
	$dir++;
    }
    printf("%7d ",$elements[4]);
    print"$elements[8]\n";
}    
close(LS);

printf("%18d file(s)%13d bytes\n",$file,$fbytes);
printf("%18d dir(s) %13d bytes\n",$dir,$dbytes);

