# This is a container used to run homebrew-ci 

# Get the homebrew ubuntu container
FROM ghcr.io/homebrew/ubuntu22.04:master

RUN mkdir -p /home/linuxbrew

# COPY chapel.rb and chapel*.tar.gz inside the container to run homebrew install 
COPY chapel.rb /home/linuxbrew/
COPY chapel*.tar.gz /home/linuxbrew/
