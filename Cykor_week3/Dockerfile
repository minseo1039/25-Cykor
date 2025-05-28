FROM php:apache

# Install dependencies
RUN apt-get update && apt-get install -y \
    && docker-php-ext-install mysqli \
    && docker-php-ext-enable mysqli

WORKDIR /var/www/html
EXPOSE 80
CMD ["apache2-foreground"]
