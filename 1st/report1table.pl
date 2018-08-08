#!/usr/local/bin/perl

$i=0;
for($m=0;$m<@ARGV;$m++){
	if($ARGV[$m]!="[A-H]"){
		print"Error(incorrect letter)\n";
		$i=1;
	}
}

if($i==0){
	if(@ARGV){
		@al=@ARGV;
	}
	else{
		@al=(A,B,C,D,E,F,G,H);
	}

	$name=join("" , @al);

	open(TABLE,"< group$al[0]");
	@line=<TABLE>;
	close(TABLE);

	chomp($line[0]);
	chomp($line[2]);

	for($i=0;$i<4;$i++){
		foreach(@team[$i]){
			$_=[split(/\s+/,$line[$i+4])];
		}
	}

	for($i=0;$i<4;$i++){
		$team[$i][0]=~ s/[_]/\\_/g;
	}

	@data=(
		[0,0,0],
		[0,0,0],
		[0,0,0],
		[0,0,0]
	);
#nチーム目の[勝ち数,負け数,引き分け数]

	for($i=0;$i<4;$i++){	#各行に対して勝ち負け引き分けを表す文字を記号で当てる
		for($k=0;$k<8;$k++){
			if($team[$i][$k]=~/^W$/){
				$team[$i][$k]="\○";
				$data[$i][0]++;
			}
			elsif($team[$i][$k]=~/^L$/){
				$team[$i][$k]="\×";
				$data[$i][1]++;
			}
			elsif($team[$i][$k]=~/^D$/){
				$team[$i][$k]="\△";
				$data[$i][2]++;
			}
		}
	}

	open(TEX,"> group-$name.tex");
	print TEX <<"EOT";	#texファイルの内容
\\documentclass[a4j,titlepage]{jarticle}
\\usepackage{multirow}
\\usepackage{plext}
\\begin{document}
\\begin{table}[htb]
\\begin{center}
\\underline{$line[0]- $line[2] -}
\\begin{tabular}{|c|c|c|c|c|c|c|c|}
\\hline
&$team[0][0]&$team[1][0]&$team[2][0]&$team[3][0]&\\pbox<t>{勝ち試合数}&\\pbox<t>{引き分け試合数}&\\pbox<t>{負け試合数}\\\\
\\hline
\\multirow{2}{*}{$team[0][0]}&&$team[0][2]&$team[0][4]&$team[0][6]&\\multirow{2}{*}{$data[0][0]}&\\multirow{2}{*}{$data[0][2]}&\\multirow{2}{*}{$data[0][1]}\\\\
&&$team[0][3]&$team[0][5]&$team[0][7]&&&\\\\
\\hline
\\multirow{2}{*}{$team[1][0]}&$team[1][1]&&$team[1][4]&$team[1][6]&\\multirow{2}{*}{$data[1][0]}&\\multirow{2}{*}{$data[1][2]}&\\multirow{2}{*}{$data[1][1]}\\\\
&$team[1][2]&&$team[1][5]&$team[1][7]&&&\\\\
\\hline
\\multirow{2}{*}{$team[2][0]}&$team[2][1]&$team[2][3]&&$team[2][6]&\\multirow{2}{*}{$data[2][0]}&\\multirow{2}{*}{$data[2][2]}&\\multirow{2}{*}{$data[2][1]}\\\\
&$team[2][2]&$team[2][4]&&$team[2][7]&&&\\\\
\\hline
\\multirow{2}{*}{$team[3][0]}&$team[3][1]&$team[3][3]&$team[3][5]&&\\multirow{2}{*}{$data[3][0]}&\\multirow{2}{*}{$data[3][2]}&\\multirow{2}{*}{$data[3][1]}\\\\
&$team[3][2]&$team[3][4]&$team[3][6]&&&&\\\\
\\hline
\\end{tabular}
\\end{center}
\\end{table}
EOT
	close(TEX);

	for($m=1;$m<@al;$m++){
		open(TABLE,"< group$al[$m]");
		@line=<TABLE>;
		close(TABLE);

		chomp($line[0]);
		chomp($line[2]);

		for($i=0;$i<4;$i++){
			foreach(@team[$i]){
				$_=[split(/\s+/,$line[$i+4])];
			}
		}

		for($i=0;$i<4;$i++){
			$team[$i][0]=~ s/[_]/\\_/g;
		}

		@data=(
			[0,0,0],
			[0,0,0],
			[0,0,0],
			[0,0,0]
		);
#nチーム目の[勝ち数,負け数,引き分け数]

		for($i=0;$i<4;$i++){
			for($k=0;$k<8;$k++){
				if($team[$i][$k]=~/^W$/){
					$team[$i][$k]="\○";
					$data[$i][0]++;
				}
				elsif($team[$i][$k]=~/^L$/){
					$team[$i][$k]="\×";
					$data[$i][1]++;
				}
				elsif($team[$i][$k]=~/^D$/){
					$team[$i][$k]="\△";
					$data[$i][2]++;
				}
			}
		}

		open(TEX,">> group-$name.tex");
		print TEX <<"EOT";
\\newpage
\\begin{table}[htb]
\\begin{center}
\\underline{$line[0]- $line[2] -}
\\begin{tabular}{|c|c|c|c|c|c|c|c|}
\\hline
&$team[0][0]&$team[1][0]&$team[2][0]&$team[3][0]&\\pbox<t>{勝ち試合数}&\\pbox<t>{引き分け試合数}&\\pbox<t>{負け試合数}\\\\
\\hline
\\multirow{2}{*}{$team[0][0]}&&$team[0][2]&$team[0][4]&$team[0][6]&\\multirow{2}{*}{$data[0][0]}&\\multirow{2}{*}{$data[0][2]}&\\multirow{2}{*}{$data[0][1]}\\\\
&&$team[0][3]&$team[0][5]&$team[0][7]&&&\\\\
\\hline
\\multirow{2}{*}{$team[1][0]}&$team[1][1]&&$team[1][4]&$team[1][6]&\\multirow{2}{*}{$data[1][0]}&\\multirow{2}{*}{$data[1][2]}&\\multirow{2}{*}{$data[1][1]}\\\\
&$team[1][2]&&$team[1][5]&$team[1][7]&&&\\\\
\\hline
\\multirow{2}{*}{$team[2][0]}&$team[2][1]&$team[2][3]&&$team[2][6]&\\multirow{2}{*}{$data[2][0]}&\\multirow{2}{*}{$data[2][2]}&\\multirow{2}{*}{$data[2][1]}\\\\
&$team[2][2]&$team[2][4]&&$team[2][7]&&&\\\\
\\hline
\\multirow{2}{*}{$team[3][0]}&$team[3][1]&$team[3][3]&$team[3][5]&&\\multirow{2}{*}{$data[3][0]}&\\multirow{2}{*}{$data[3][2]}&\\multirow{2}{*}{$data[3][1]}\\\\
&$team[3][2]&$team[3][4]&$team[3][6]&&&&\\\\
\\hline
\\end{tabular}
\\end{center}
\\end{table}
EOT
		close(TEX);
	}

	open(TEX,">> group-$name.tex");
	print TEX "\\end{document}";
	close(TEX);
}