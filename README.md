# TextTok 

I am keeping this readme, along with all commits (which remain unsquashed) to make my actions in developing this software transparent. 

## Initial analysis 

I have not encountered `Tok` orn `TextTok` as protocols, but the [`public-api.md`](texttok/pulic-api.md) doc refers to `Tox`. A search seams to suggest that this is a protocol ([TokTok](https://toktok.ltd/spec.html)), but unsure if this is related.

## Initial development

It would appear that I can grab the latest ID of a `Tox`, then pull the tox by id for the server. Currentlyu this is done by numerous calls to the sever for basic types (uint8_t, uint32_t, etc). While this is fine as TCP is a stream portocol, it might be more efficient to make fewer calls and package everything up in packed structures to create basic packets. 

The socket component was developed without unit tests simply because doing so would have necesitated the creation of a socke server mimicing the protocol being used. While this would have no doubt been iluminating, it might also have been time consuming and lead to a dead end.