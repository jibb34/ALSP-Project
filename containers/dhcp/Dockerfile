FROM ubuntu:latest
RUN apt-get update && apt-get install -y isc-dhcp-server
COPY dhcpd.conf /etc/dhcp/dhcpd.conf

EXPOSE 67/udp
RUN mkdir -p /var/lib/dhcp && touch /var/lib/dhcp/dhcpd.leases
CMD ["/usr/sbin/dhcpd", "-4", "-cf", "/etc/dhcp/dhcpd.conf", "-f"]
