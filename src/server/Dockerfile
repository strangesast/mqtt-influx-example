from node:slim

workdir /usr/src/app
copy package*.json ./
run npm ci --production
copy . .

cmd ["node", "server.js"]
