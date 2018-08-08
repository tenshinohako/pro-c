#!/usr/local/bin/perl
#
# Very Simple Editor
#

use Tk;

$mw = MainWindow->new();
$mw->title("Very Simple Editor");

#
# ボタンとファイル名エントリを格納するフレームを作成
#
$top = $mw->Frame(-borderwidth => 10);
$top->pack(-side => 'top', -fill => 'x');

#
# ボタンとファイル名エントリを作成, 配置
#
$quit = $top->Button(-text => 'Quit', -command => sub { exit });
$clear = $top->Button(-text => 'Clear', -command => \&clear_text);
$save = $top->Button(-text => 'Save', -command => \&save_text);
$load = $top->Button(-text => 'Load', -command => \&load_text);
$insert = $top->Button(-text => 'Insert', -command => \&insert_text);
$label = $top->Label(-text => 'Filename:', -padx => 0);
$filename = $top->Entry(-width => 20,
			-relief => 'sunken',
			-textvariable => \$savefilename);
$label->pack(-side => 'left');
$filename->pack(-side => 'left');
$quit->pack(-side => 'right');
$clear->pack(-side => 'right');
$save->pack(-side => 'right');
$load->pack(-side => 'right');
$insert->pack(-side => 'right');

#
# エディットに利用するテキストウィジェットを作成
#
$textframe = $mw->Frame();
$scroll = $textframe->Scrollbar();
$textfield = $textframe->Text(-width => 80,
			      -height => 25,
			      -borderwidth => 2,
			      -relief => 'sunken',
			      -setgrid => 1,
			      -yscrollcommand => ['set' => $scroll]);
$scroll->configure(-command => ['yview' => $textfield]);
$scroll->pack(-side => 'right', -fill => 'y');
$textfield->pack(-side => 'left', -fill => 'both', -expand => 1);
$textframe->pack(-side => 'top', -fill => 'both', -expand => 1);

#
# 表示
#
MainLoop;

#
# Clearボタンに対応する手続き
# エディット中のテキストをすべて削除する
#
sub clear_text {
    $textfield->delete('1.0','end');
}

#
# Saveボタンに対応する手続き
# filenameエントリに記述されたファイル名でファイルをオープンし, 
# テキストウィジェットの内容をそのファイルへ書き込む
#
sub save_text {
    open(OUTFILE, ">$savefilename")
	or warn("Can't open '$savefilename'"), return;
    print OUTFILE $textfield->get('1.0','end');
    close(OUTFILE);
}

sub load_text{
    open(INFILE, "<$savefilename")
	or warn("Can't open '$savefilename'"), return;
    $textfield->delete( '1.0', 'end' );
    while(<INFILE>){
	$textfield->insert( 'end', $_ );
    }
    close(INFILE);
}

sub insert_text{
    open(INFILE, "<$savefilename")
	or warn("Can't open '$savefilename'"), return;
    while(<INFILE>){
	$textfield->insert( 'insert', $_ );
    }
    close(INFILE);
}