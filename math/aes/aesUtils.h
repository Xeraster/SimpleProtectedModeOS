    //utilities to make aes easier to interface with c++ os
    //use this one for rsa:
    //https://github.com/abumq/mine
    #ifndef AESUTILS
    #define AESUTILS

        //convert a string to a suitable 
        char *keyFromString(string input)
        {
            char *newKey = (char*)malloc(AES_KEYLEN);
            //first, pad with zeros
            for (int i = 0; i < AES_KEYLEN; i++)
            {
                newKey[i] = 0;
            }

            for (int i = 0; i < input.length(); i++)
            {
                newKey[i%AES_KEYLEN] = (newKey[i%AES_KEYLEN] + input.at(i));
            }

            //return the address of the char array with the key. Be sure to free the pointer when done using it
            return newKey;
        }
        //input a string and an encryption key, get an encrypted string back
        string encrypt(string input, unsigned char *key)
        {
            //initialize the stuff
            struct AES_ctx ctx;
            unsigned char iv[]  = { (unsigned char)0x00, (unsigned char)0x01, (unsigned char)0x02, (unsigned char)0x03, (unsigned char)0x04, (unsigned char)0x05, (unsigned char)0x06, (unsigned char)0x07, (unsigned char)0x08, (unsigned char)0x09, (unsigned char)0x0a, (unsigned char)0x0b, (unsigned char)0x0c, (unsigned char)0x0d, (unsigned char)0x0e, (unsigned char)0x0f };
            AES_init_ctx_iv(&ctx, key, iv);

            //figure out the block numbers and stuff
            unsigned int numBlocks = (input.length() / AES_BLOCKLEN);
            if ((input.length() % AES_BLOCKLEN) > 0)
            {
                numBlocks++;
            }
            
            string encrypted = "";
            //for each block of AES_BLOCKLEN bytes, do it all
            /*for (int i = 0; i < numBlocks; i++)
            {
                //for (int p = 0; p < )
                char *someByte = (char*)malloc(AES_BLOCKLEN);
                for (int p = 0; p < AES_BLOCKLEN; p++)
                {
                    if ((p + (i*AES_BLOCKLEN)) < input.length())
                    {
                        someByte[p] = input.at(p + (i*AES_BLOCKLEN));
                    }
                    else
                    {
                        someByte[p] = 0;
                    }
                }

                AES_CBC_encrypt_buffer(&ctx, someByte, AES_BLOCKLEN);    //let's try CBC this time since its better
                for (int c = 0; c < AES_BLOCKLEN; c++)
                {
                    //encrypted[(i*AES_BLOCKLEN)+c]=someByte[c];
                    encrypted+=someByte[c];
                }
                free(someByte);
            }*/

            unsigned char *someByte = (unsigned char *)malloc(AES_BLOCKLEN * numBlocks);
            for (int i = 0; i < numBlocks * AES_BLOCKLEN; i++)
            {
                if (i < input.length())
                {
                    someByte[i] = input.at(i);
                }
                else
                {
                    someByte[i] = 0;
                }
            }
            AES_CBC_encrypt_buffer(&ctx, someByte, numBlocks); // let's try CBC this time since its better
            for (int c = 0; c < AES_BLOCKLEN*numBlocks; c++)
            {
                // encrypted[(i*AES_BLOCKLEN)+c]=someByte[c];
                encrypted += someByte[c];
            }
            free(someByte);

            return encrypted;
        }

        //input an encrypted string and encryption key, get a hopefully decrypted string back
        string decrypt(string input, unsigned char *key)
        {
            //initialize the stuff
            struct AES_ctx ctx;
            unsigned char iv[]  = { (unsigned char)0x00, (unsigned char)0x01, (unsigned char)0x02, (unsigned char)0x03, (unsigned char)0x04, (unsigned char)0x05, (unsigned char)0x06, (unsigned char)0x07, (unsigned char)0x08, (unsigned char)0x09, (unsigned char)0x0a, (unsigned char)0x0b, (unsigned char)0x0c, (unsigned char)0x0d, (unsigned char)0x0e, (unsigned char)0x0f };
            AES_init_ctx_iv(&ctx, key, iv);

            //figure out the block numbers and stuff
            unsigned int numBlocks = (input.length() / AES_BLOCKLEN);
            if ((input.length() % AES_BLOCKLEN) > 0)
            {
                numBlocks++;
            }
            
            string decrypted = "";
            //for each block of AES_BLOCKLEN bytes, do it all
            /*for (int i = 0; i < numBlocks; i++)
            {
                //for (int p = 0; p < )
                char *someByte = (char*)malloc(AES_BLOCKLEN);
                for (int p = 0; p < AES_BLOCKLEN; p++)
                {
                    if ((p + (i*AES_BLOCKLEN)) < input.length())
                    {
                        someByte[p] = input.at(p + (i*AES_BLOCKLEN));
                    }
                    else
                    {
                        someByte[p] = 0;
                    }
                }

                AES_CBC_decrypt_buffer(&ctx, someByte, AES_BLOCKLEN);    //let's try CBC this time since its better
                for (int c = 0; c < AES_BLOCKLEN; c++)
                {
                    //encrypted[(i*AES_BLOCKLEN)+c]=someByte[c];
                    decrypted+=someByte[c];
                }
                free(someByte);
            }*/

            unsigned char *someByte = (unsigned char *)malloc(AES_BLOCKLEN * numBlocks);
            for (int i = 0; i < numBlocks * AES_BLOCKLEN; i++)
            {
                if (i < input.length())
                {
                    someByte[i] = input.at(i);
                    //decrypted += someByte[i];
                }
                else
                {
                    someByte[i] = 0;
                    //decrypted += ".";
                }
                //someByte[i] = input.at(i);
            }
            AES_CBC_decrypt_buffer(&ctx, someByte, numBlocks); // let's try CBC this time since its better
            for (int c = 0; c < AES_BLOCKLEN*numBlocks; c++)
            {
                // encrypted[(i*AES_BLOCKLEN)+c]=someByte[c];
                decrypted += someByte[c];
            }
            free(someByte);

            return decrypted;
        }

    #endif