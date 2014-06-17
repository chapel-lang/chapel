
"""
Contains a function to make an html movie out of a sequence of plotfiles.

Written by R.J. LeVeque based on template from Alan McIntyre.
"""


def make_movie(plotfiles, moviename='plotmovie.html'):
    """ 
    Takes a list plotfiles which should be for example of the form:
        ['frame00.png', 'frame01.png', ... ]
    where each string should be the name of an image file and they should be
    in the proper order for viewing as an animation.

    The result is an html file with name specified by moviename
    that incorporates javascript to loop through the plots one after
    another.  The html page also has buttons for controlling the movie.
    The parameter iterval below is the time interval between loading
    successive images and is in milliseconds.

    The img_width and img_height parameters do not seem to have any effect
    for reasons not understood.
    """

    import os
    text = """
           <html>
           <head>
           <script language="Javascript">
           <!---
           var num_images = %s; """ % len(plotfiles)

    text += """
           var img_width = 800;
           var img_height = 600;
           var interval = 300;    
           var images = new Array();

        function preload_images()
        {
            t = document.getElementById("progress");
            """

    i = 0
    for fname in plotfiles[1:]:
        i = i+1
        text += """
        t.innerHTML = "Preloading image ";
        images[%s] = new Image(img_width, img_height);
        images[%s].src = "%s";
        """ % (i,i,fname)
    text += """
        t.innerHTML = "";
        }

        function tick()
        {
          frame += 1;
          if (frame > num_images)
              frame = 1;

          document.movie.src = images[frame].src;
          tt = setTimeout("tick()", interval);
        }

        function startup()
        {
          preload_images();
          frame = 0;
          setTimeout("tick()", interval);
        }

        function stopit()
        {
          clearTimeout(tt);
        }
        function restart()
        {
          tt = setTimeout("tick()", interval);
        }
        function slower()
        {
          interval = interval / 0.7;
        }
        function faster()
        {
          interval = interval * 0.7;
        }
        // --->
        </script>
        </head>
        <body>

        <form>
        &nbsp;
        <input type="button" value="Start movie" onClick="startup()">
        <input type="button" value="Pause movie" onClick="stopit()">
        <input type="button" value="Restart movie" onClick="restart()">
        &nbsp;
        <input type="button" value="Slower" onClick="slower()">
        <input type="button" value="Faster" onClick="faster()">
        </form>

        <p><div ID="progress"></div></p>
        <img src="%s" name="movie"/>
        </body>
        </html>
        """ % plotfiles[0]

    htmlfile = open(moviename,'w')
    htmlfile.write(text)
    htmlfile.close()
    print "To view animation, point your browser to:"
    print "   file://" + os.path.abspath(moviename)
    # end of htmlmovie

