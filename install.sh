#!/bin/bash

# wget https://repo.anaconda.com/archive/Anaconda3-2021.05-Linux-x86_64.sh
# bash Anaconda3-2021.05-Linux-x86_64.sh -b -p /home/ubuntu/.anaconda
# ... do ~/.bashrc stuff ...
# conda init bash

conda create -y -n delphi_env python=3.8
conda activate delphi_env

conda install -y cmake

sudo apt-get update
sudo apt-get -y --no-install-recommends install \
      build-essential \
      libboost-all-dev \
      pkg-config \
      curl \
      git \
      tar \
      wget \
      doxygen \
      graphviz \
      libgraphviz-dev \
      libsqlite3-dev \
      libeigen3-dev \
      pybind11-dev \
      libfmt-dev \
      librange-v3-dev

sudo apt-get -y install nlohmann-json3-dev

pip install -e . 
pip install matplotlib --ignore-installed

# COPY . /delphi
# WORKDIR /delphi

# RUN python3 -m venv $VIRTUAL_ENV

# RUN mkdir -p data && curl http://vanga.sista.arizona.edu/delphi_data/delphi.db -o data/delphi.db
# RUN . $VIRTUAL_ENV/bin/activate && pip install wheel && pip install -e .
# CMD delphi_rest_api
