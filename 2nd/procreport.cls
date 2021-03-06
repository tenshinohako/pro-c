\NeedsTeXFormat{pLaTeX2e}
\ProvidesClass{procreport}
\LoadClass[fleqn,10pt,a4paper]{jarticle}
\def\@$B=jB0(B{}
\newcommand{\$B=jB0(B}[1]{\def\@$B=jB0(B{#1}}
\def\@$B3XG/(B{}
\newcommand{\$B3XG/(B}[1]{\def\@$B3XG/(B{#1}}
\def\@$B3X@RHV9f(B{}
\newcommand{\$B3X@RHV9f(B}[1]{\def\@$B3X@RHV9f(B{#1}}
\def\@$BC4Ev650w(B{}
\newcommand{\$BC4Ev650w(B}[1]{\def\@$BC4Ev650w(B{#1}}
\def\@email{}
\newcommand{\email}[1]{\def\@email{#1}}
\renewcommand{\maketitle}{%
\begin{titlepage}
\vspace*{7cm}
\begin{center}
  % {\LARGE \@title}\\
  {\LARGE $B%W%m%0%i%_%s%0(BC$B1i=,Js9p=q(B}\\
  \bigskip
  {\LARGE \@title}\\
  \medskip
  {\large $B!ZC4Ev650w![(B\quad\@$BC4Ev650w(B\quad $B650w(B}
\end{center}
\vspace*{9cm}
\begin{large}
\begin{tabbing}
$B%^!<%8%s(B\hspace{6cm}\=$B$"9`L\(B====\=  $BFbMF(B\kill
                      \> $B!ZDs=P<T![(B\> \@author\quad (\@$B3X@RHV9f(B)\\
                      \>          \> \@$B=jB0!&(B\@$B3XG/(B\\
                      \>          \> \@email\\
                      \> $B!ZDs=PF|![(B\> \@date
\end{tabbing}
\end{large}
\end{titlepage}}
\usepackage[text={16cm,24cm},centering,dvipdfm]{geometry}
\usepackage{times}
\setlength{\parskip}{1ex plus 0.5ex minus 0.2ex}
