/<div class=\"line\">.*/{
  s/ /\&nbsp;/g
  s/<[^>]\{1,\}>//g
  s/$/<br\/>/
}
s/\&ndash;/--/g
s/©/(c)/g
s/é/&eacute;/g
p
