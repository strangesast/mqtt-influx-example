from node as builder

workdir /usr/src/app
copy src/client/package*.json ./
run npm install
copy src/client/ .
run ./node_modules/.bin/ng build --prod


from nginx

copy --from=builder /usr/src/app/dist/app /usr/share/nginx/html/
copy etc/nginx/templates/default.conf.template /etc/nginx/templates/
