FROM ubuntu:latest

RUN apt-get update && apt-get install -y tftpd-hpa

RUN mkdir -p /srv/tftp
RUN chmod -R 777 /srv/tftp

RUN echo 'TFTP_USERNAME="tftp' > /etc/default/tftpd-hpa && \
  echo 'TFTP_DIRECTORY="/srv/tftp"' >> /etc/default/tftpd-hpa && \
  echo 'TFTP_ADDRESS="0.0.0.0:69"' >> /etc/default/tftpd-hpa && \
  echo 'TFTP_OPTIONS="--secure"' >> /etc/default/tftpd-hpa
COPY bootfiles/ srv/tftp/

EXPOSE 69/udp

CMD ["/usr/sbin/in.tftpd", "--foreground", "--secure", "/srv/tftp"]

