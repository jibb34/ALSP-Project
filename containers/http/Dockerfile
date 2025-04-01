FROM nginx:latest

WORKDIR /usr/share/nginx/html

# Copy custom NGINX config file
COPY ../../config/http.conf /etc/nginx/nginx.conf

# Copy files to be served by HTTP server
COPY ../../httpfiles/ /usr/share/nginx/html/

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]

