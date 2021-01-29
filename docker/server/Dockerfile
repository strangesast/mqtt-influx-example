from node

workdir /usr/src/app
copy src/server/package*.json ./
run npm ci --production
copy src/server .

cmd ["node", "server.js"]
