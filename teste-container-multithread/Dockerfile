FROM gcc:4.9
COPY . /Codigo
WORKDIR /Codigo/
RUN gcc -o filosofos codigo.c -lpthread
CMD ["./filosofos"]