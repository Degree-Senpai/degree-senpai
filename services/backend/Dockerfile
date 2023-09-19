FROM python:3.11-buster

RUN mkdir app
WORKDIR /app

RUN apt-get update && \
    apt-get -y install gcc

ENV PATH="${PATH}:/root/.local/bin"
ENV PYTHONPATH=.

COPY requirements.txt .
RUN pip install --upgrade pip
RUN pip install -r requirements.txt



COPY src/ .
