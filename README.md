# CRYPTOPALS Solution!
This is a solution for the [crypopals](https://cryptopals.com) challenges.
* In the repo you can find solutions for all the challenges, all merged into the master branch.
* All the code is written in pure c++
* All the code is tested using gtest
* Beside gtest, there is no use of any other external library (including mathematical ones).

## Models
Beside the attacks and challenges, this repo contains implementations for:
* **Block Cipher** - AES128/192/256
* **Modes** - CBC/ECB/CTR
* **Hashes** - SHA1 and MD5
* **Key Exange** - Diffie-Hellman key exange
* **Mathematics** - bignums, modpow, modmul
* **Others** - encoding, padding, base64, hex and other utils

## Challenges Comments
* **5.1** - The implementation of the modmul and modpow is not the best... its no the trivial implementations but are far from optimal and takes alot of memory and time to calculate. There are much better ways for implementing this....
