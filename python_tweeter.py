#!/usr/bin/env python

#requires updated tweepy for statuses with media (do a pip install tweepy --upgrade)


import tweepy, sys, random, time


# insert twitter auth keys here
auth=tweepy.OAuthHandler(c_k, c_s)
# and here the access token
auth.set_access_token(a_t, a_s)
api = tweepy.API(auth)

time.sleep(2);

photo_path = sys.argv[1]
status = random.choice(open('quotes.txt').readlines()) + " - Facce da #Codemotion"
api.update_with_media(photo_path, status, lat="41.8548728", long="12.4699596")

print "tweet: ", photo_path, status
