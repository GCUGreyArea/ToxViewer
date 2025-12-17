# TextTok 

## Initial analysis 

I have not encountered `Tok` orn `TextTok` as protocols, but the [`public-api.md`](texttok/pulic-api.md) doc refers to `Tox`. A search seams to suggest that this is a protocol ([TokTok](https://toktok.ltd/spec.html)), but unsure if this is related.

It would appear that I can grab a list of toxes, then pull the tox by id, then read the tox as this is in UTF-8, then rate those toxes. The simplest way foward, given that it's a TCP/IP service, is to build a simple client and see what we get. This will be a command line driven C++ client based on a simple class. Possibly a mistake as Python might be faster to develop