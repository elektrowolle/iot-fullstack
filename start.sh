#!/usr/bin/env bash
node-red "./node-red" >> ./stackLog &
cd server
npm start >> ../stackLog &
cd ../client
npm run dev >> ../stackLog &