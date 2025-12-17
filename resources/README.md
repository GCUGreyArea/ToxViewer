# TextTok 

I am keeping this readme, along with all commits (which remain unsquashed) to make my actions in developing this software transparent. 

## Initial analysis 

I have not encountered `Tok` orn `TextTok` as protocols, but the [`public-api.md`](texttok/pulic-api.md) doc refers to `Tox`. A search seams to suggest that this is a protocol ([TokTok](https://toktok.ltd/spec.html)), but unsure if this is related.

## Initial development

It would appear that I can grab the latest ID of a `Tox`, then pull the tox by id for the server. Currentlyu this is done by numerous calls to the sever for basic types (uint8_t, uint32_t, etc). While this is fine as TCP is a stream portocol, it might be more efficient to make fewer calls and package everything up in packed structures to create basic packets. 

The socket component was developed without unit tests simply because doing so would have necesitated the creation of a socke server mimicing the protocol being used. While this would have no doubt been iluminating, it might also have been time consuming and lead to a dead end.

## Further development 

1. Currently we pull each tox and display it. There is no way to rate the tox (thumbs up, thumbs down). 
2. The protocol also does not allow us to pull a list of all availible toxes. This implies 
    1. We need to test that a tox can be retrieved later (after we have pulled another tox), and that it can be still be rated. Ideally we would like to get all toxes before we rate them, though the list of availible toxes could be very big?
3. Tests have shown that it is possible to get the same tox on subsequent calls. We need take that into account. 
4. There seems to be no way to request the current ratting for a tox? There is probably a hiden API. I've tried voting with a value of 0, but this blocks teh socket. One way to do it would be to simply upvote, then downvote the tox, and store the returned value. This is not programatically clean however!