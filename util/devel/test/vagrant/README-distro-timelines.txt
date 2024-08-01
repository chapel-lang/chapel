x means reached end of life
- means end of life soon
  (last updated: 2024-06-13)

Alma Linux -- see https://wiki.almalinux.org/release-notes/
           -- EOL dates probably match Rocky linux
  8.x EOL 2029
  9.x EOL 2032
x 9.0 released May 2022
x 9.1 released Nov 2022
x 9.2 released May 2023
  9.3 released Nov 2023
  9.4 released May 2024

Alpine Linux -- see https://www.alpinelinux.org/releases/
x 3.14  EOL 2023-05-01
x 3.15  EOL 2023-11-01
x 3.16  EOL 2024-05-23
  3.17  EOL 2024-11-22
  3.18  EOL 2025-05-09
  3.19  EOL 2025-11-01
  3.20  EOL 2026-04-01

Amazon Linux
       -- see https://docs.aws.amazon.com/linux/al2022/ug/release-cadence.html
       -- which has 2 years of standard support, 3 of maintenance
  2    EOL June 30, 2025
x 2022 was renamed to 2023 in March 2023
  2023 EOL 2028
  2025 to be released

Arch
* currently just testing the current release

CentOS -- see https://wiki.centos.org/About/Product
x 5 full updates until Jan 2014, maintenance until Mar 2017
x 6 full updates until May 2017, maintenance until Nov 2020
x   6.9      released Apr 2017
x   6.10     released Jul 2018
- 7 full updates until Dec 2020, maintenance until Jun 2024
x   7.3      released Dec 2016
x   7.4      released Sep 2017
x   7.5      released May 2018
x   7.6      released Dec 2018
x   7.7      released Sep 2019
x   7.8-2003 released Apr 2020
-   7.9-2009 released Nov 2020 EOL Jun 2024
    # note -- gcc 11 is available with devtoolset-11
      https://stackoverflow.com/questions/36327805/how-to-install-gcc-5-3-with-yum-on-centos-7-2/37355018
x 8 not-stream - EOL 2021-12-31
    (CentOS only available in Stream releases after that)

CentOS Stream
       -- dnf doesn't old show versions by default but they can be found, e.g.
       -- dnf list --showduplicates llvm && dnf install llvm-devel-14.0.6
x 8 - EOL May 2024
  9 - EOL May 2027


Debian -- see https://wiki.debian.org/DebianReleases
       -- and https://app.vagrantup.com/debian
x  6 "squeeze"  long-term support until Feb 2016
x  7 "wheezy"   long-term support until May 2018
x  8 "jessie"   long-term support until May 2020
x  9 "stretch"  long-term support until Jun 2022
- 10 "buster"   long-term support until Jun 2024
  11 "bullseye" long-term support until Jun 2026
  12 "bookworm" released June 2023
  13 "trixie"   expected release ?
  14 "forky"    expected release ?

Fedora -- see https://en.wikipedia.org/wiki/Fedora_Linux_release_history
       -- and https://fedoraproject.org/wiki/End_of_life
x 21 EOL Dec 2015
x 22 EOL Jul 2016
x 23 EOL Dec 2016
x 24 EOL Aug 2017
x 25 EOL Dec 2017
x 26 EOL May 2018
x 27 EOL Nov 2018
x 28 EOL May 2019
x 29 EOL Nov 2019
x 30 EOL May 2020
x 31 EOL Nov 2020
x 32 EOL May 2021
x 33 EOL Nov 2021
x 34 EOL Jun 2022
x 35 EOL Dec 2022
x 36 EOL May 2023
x 37 EOL Nov 2023
x 38 EOL May 2024
- 39 EOL Nov 2024
  40 EOL May 2025
  41 release Oct 2024

FreeBSD -- see https://www.freebsd.org/security/unsupported.html
        -- and https://app.vagrantup.com/freebsd
        -- also note https://docs.freebsd.org/en/books/handbook/virtualization/#virtualization-guest-virtualbox
           has instructions for installing virtualbox guest additions, in
           case that is needed
x 10.2 EOL Dec 2016
x 10.3 EOL Apr 2018
x 10.4 EOL Oct 2018
x 11.0 EOL Nov 2017
x 11.1 EOL Sep 2018
x 11.2 EOL Oct 2019
x 11.4 EOL Sep 2021
x 12.0 EOL Feb 2020
x 12.1 EOL Jan 2021
x 12.2 EOL Mar 2022
x 12.3 EOL Mar 2023
x 12.4 EOL Dec 2023
x 13.0 EOL Aug 2022
x 13.1 EOL Jul 2023
- 13.2 EOL Jul 2024
  13.3 released Mar 2024
  14.0 released Nov 2023
  14.1 released Jun 2024

OpenSuse -- see https://en.opensuse.org/Lifetime
         -- and https://app.vagrantup.com/opensuse
x 13.1 EOL Feb 2016 / evergreen in Nov 2016
x 13.2 EOL Jan 2017
x 42.1 EOL May 2017
x 42.2 EOL Jan 2018
x 42.3 EOL Jun 2019
x 15.0 EOL Nov 2019
x 15.1 EOL Nov 2020
x 15.2 EOL Dec 2021
x 15.3 EOL Nov 2022
x 15.4 EOL Nov 2023
  15.5 EOL Dec 2024
  15.6 EOL Dec 2025

Rocky Linux -- see https://wiki.rockylinux.org/rocky/version/
  8.x EOL May 2029
x 8.8 EOL Nov 2023
x 8.9 EOL May 2024
  8.10 EOL May 2029

  9.x EOL May 2032
x 9.0 EOL Nov 2022
x 9.1 EOL May 2023
x 9.2 EOL Nov 2023
x 9.3 EOL May 2024
  9.4 EOL May 2032

Ubuntu -- see https://wiki.ubuntu.com/Releases
       -- and https://app.vagrantup.com/ubuntu
LTS:
x 12.04 "Precise Pangolin"  LTS until Apr 2017, EOL Apr 2017
x 14.04 "Trusty Tahir"      LTS until Apr 2019, EOL Apr 2024
x 16.04 "Xenial Xerus"      LTS until Apr 2021, EOL Apr 2026
x 18.04 "Bionic Beaver"     LTS until Jun 2023, EOL Apr 2028
  20.04 "Focal Fossa"       LTS until Apr 2025, EOL Apr 2030
  22.04 "Jammy Jellyfish"   LTS until Jun 2027, EOL Apr 2032
  24.04 "Noble Numbat"      LTS until Jun 2029, EOL Apr 2036

non-LTS:
x 14.10 "Utopic Unicorn"    EOL July 2015
x 15.04 "Vivid Vervet"      EOL Feb 2016
x 15.10 "Wily Werewolf"     EOL July 2016
x 16.10 "Yakkety Yak"       EOL July 2017
x 17.04 "Zesty Zapus"       EOL Jan 2018
x 17.10 "Artful Aardvark"   EOL July 2018
x 18.10 "Cosmic Cuttlefish" EOL July 2019
x 19.04 "Disco Dingo"       EOL Jan 2020
x 19.10 "Eoan Ermine"       EOL July 2020
x 20.10 "Groovy Gorilla"    EOL July 2021
x 21.04 "Hirsute Hippo"     EOL Jan 2022
x 21.10 "Impish Indri"      EOL July 2022
x 22.10 "Kinetic Kudu"      EOL July 2023
x 23.04 "Lunar Lobster"     EOL Jan 2024
- 23.10 "Mantic Minotaur"   EOL Jul 2024
  24.10 "Oracular Oriole"   EOL Jul 2025 (to be released)
