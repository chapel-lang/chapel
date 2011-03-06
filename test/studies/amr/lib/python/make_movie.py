import html_movie
plotfiles = []
for n in range(21):
    fname = "AdvectionCTU_frame%s.png" % str(n).rjust(4,'0')
    plotfiles.append(fname)

html_movie.make_movie(plotfiles, "AdvectionCTU_movie.html")
