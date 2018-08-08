\NeedsTeXFormat{pLaTeX2e}
\ProvidesClass{procreport}
\LoadClass[fleqn,10pt,a4paper]{jarticle}
\def\@所属{}
\newcommand{\所属}[1]{\def\@所属{#1}}
\def\@学年{}
\newcommand{\学年}[1]{\def\@学年{#1}}
\def\@学籍番号{}
\newcommand{\学籍番号}[1]{\def\@学籍番号{#1}}
\def\@担当教員{}
\newcommand{\担当教員}[1]{\def\@担当教員{#1}}
\def\@email{}
\newcommand{\email}[1]{\def\@email{#1}}
\renewcommand{\maketitle}{%
\begin{titlepage}
\vspace*{7cm}
\begin{center}
  % {\LARGE \@title}\\
  {\LARGE プログラミングC演習報告書}\\
  \bigskip
  {\LARGE \@title}\\
  \medskip
  {\large 【担当教員】\quad\@担当教員\quad 教員}
\end{center}
\vspace*{9cm}
\begin{large}
\begin{tabbing}
マージン\hspace{6cm}\=あ項目====\=  内容\kill
                      \> 【提出者】\> \@author\quad (\@学籍番号)\\
                      \>          \> \@所属・\@学年\\
                      \>          \> \@email\\
                      \> 【提出日】\> \@date
\end{tabbing}
\end{large}
\end{titlepage}}
\usepackage[text={16cm,24cm},centering,dvipdfm]{geometry}
\usepackage{times}
\setlength{\parskip}{1ex plus 0.5ex minus 0.2ex}
