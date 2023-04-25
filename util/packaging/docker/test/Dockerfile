# This is a container used to run homebrew-ci 

# Get the ubuntu container
FROM homebrew/ubuntu20.04:latest

RUN mkdir -p /home/linuxbrew

# COPY chapel.rb and chapel*.tar.gz inside the container to run homebrew install 
COPY chapel.rb /home/linuxbrew/
COPY chapel*.tar.gz /home/linuxbrew/


