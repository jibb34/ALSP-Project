# Base Image
FROM ubuntu:latest

# Install TFTP server
RUN apt-get update && apt-get install -y tftpd-hpa

# Create TFTP root directory
RUN mkdir -p /srv/tftp

# Configure TFTP server (Write entire file in one go)
RUN echo 'TFTP_USERNAME="tftp"\nTFTP_DIRECTORY="/srv/tftp"\nTFTP_ADDRESS="0.0.0.0:69"\nTFTP_OPTIONS="--secure"' > /etc/default/tftpd-hpa

# Copy bootfiles (dynamically generated before build)
COPY bootfiles/ /srv/tftp/

# Expose TFTP port
EXPOSE 69/udp

# Run TFTP server
CMD ["/usr/sbin/in.tftpd", "--foreground", "--secure", "/srv/tftp"]

