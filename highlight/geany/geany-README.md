# Geany syntax highlighting

Nelson Luís Dias (<nelsonluisdias@gmail.com>; <https://nldias.github.io>).  
2021-02-27T12:14:00

Here is a small but usable Geany ([https://geany.org](URL))  syntax highlighting scheme for Chapel.  It was directly modified from the standard Geany (version 1.36) syntax highlighting for C.

**Windows users**: Replace `~/.config/geany` with `C:\Users\"user"\AppData\Roaming\geany`,
where "user" is your username.

Instructions:

   1. Copy `filetype.Chapel.conf` to `~/.config/geany/filedefs/`

   2. Either:
      1.  Copy filetype_extensions.conf to `~/.config/geany` OR
      2.  Include the following two lines into `~/.config/geany/filetype_extensions.conf`, if it     already exists:
        * under `[Extensions]:`

               `Chapel=*.chpl;`

        * under `[Groups]:`

               `Programming=Chapel;`

3. Close and reload Geany.

**Note that these actions will not by themselves make the Geany IDE work, i.e., the buttons for compilation and building will not work if the path to the Chapel compiler is not recognized by Geany**

