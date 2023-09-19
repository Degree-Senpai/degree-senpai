FROM python:3.8-slim

RUN groupadd -r celery && useradd -r -g celery celery
ENV PYTHONPATH=/usr/src/api

WORKDIR /app
COPY . /app

RUN pip install -r requirements.txt

USER celery

RUN apt-get update && \
    apt-get -y install gcc

CMD celery -A celery_app worker --loglevel=info
