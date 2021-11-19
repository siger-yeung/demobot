FROM ubuntu:18.04
ARG USER=user
ARG UID=1000
ARG GID=1000
RUN apt-get update
RUN apt-get -y install wget curl gcc-7-multilib g++-7-multilib
RUN groupadd -g $GID -o $USER
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $USER

RUN apt-get -y install git build-essential libreadline-dev ccache
USER $USER
RUN bash -c "bash <(curl -fsSL https://xmake.io/shget.text)"