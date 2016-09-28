\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{spec}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{book}}

\ProcessOptions

\LoadClass{book}

\renewcommand{\@makechapterhead}[1]{%
  { \parindent \z@ \raggedright 
    \normalfont \huge\bfseries
    \ifnum \c@secnumdepth >\m@ne
      \if@mainmatter
        \thechapter
		\space\space\space\space
      \fi
    \fi
    #1\par\nobreak
    \vskip 40\p@
  }
}

\renewcommand{\@makeschapterhead}[1]{%
  {\parindent \z@ \raggedright
    \normalfont \huge \bfseries
    #1\par\nobreak
    \vskip 40\p@
  }
}

\endinput
