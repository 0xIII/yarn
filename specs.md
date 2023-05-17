# The Yarn Protocol Specification

Yarn is an experimental `Peer-To-Peer`, `Plain Text Protocol` aimed at providing:
- **complete decentralisation** with a minimal number of root servers
- **attribution** and **mutability**
- a **human readable** protocol
- a **reliable/fast** connection
- an **Internet Protocol independent** communication scheme

Implementations should also provide a **low memory overhead** and **minimise energy consumption**.

The basis of this protocol is the **asymmetric encryption** scheme which is used to ensure the **attribution** and **mutability** of data. Data or `resources` are identified by a cryptographically secure keypair used to address the data it is associated with and verify its validity utilising the `signature` stored with every `resource`. A `local cache` and a `remote cache` are kept alongside a `peer-list` to reduce the amount of time it takes to acquire a requested `resource`.

## Packet Format

DELIM	:	"CRLF"
VERSION	:	"YARN/1.0"
REQTYPE	:	"GET" | "PUT" | "SYNC" | "ADD" | "REMOVE" | "PING" | "MIRROR"

REQ		:	(REQTYPE)		(IDENT)		(VERSION)	(DELIM)
RESP	:	(RESPTYPE)		()						(DELIM)
HEADER	:	(HDRTYPE)		(":")		(DATA)		(DELIM)

BODY	:	(DATA)			(DELIM)

PACKET	:	(REQ | RESP)	([HEADER]*)	(DELIM)		(BODY)

## Requests

### Request Types

#### GET
The GET request is used to requests a `resource` from other peers on the network.
```
┌─────────┬─────────────────────────┬────────────┐
│ GET     │ Public Key              │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
└─────────┴──────────────────────────────────────┘
```

####  PUT
ask peers to host a given `resource`
```
┌─────────┬─────────────────────────┬────────────┐
│ PUT     │ Public Key              │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
└─────────┴──────────────────────────────────────┘
┌────────────────────────────────────────────────┐
│ SIGNATURE                                      │
└────────────────────────────────────────────────┘
```

#### UPDATE
ask peers to update a `resource` with new data (requires private key)
```
┌─────────┬─────────────────────────┬────────────┐
│ PUT     │ Public Key              │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
├─────────┴──────────────────────────────────────┤
| Data                                           |
└────────────────────────────────────────────────┘
┌────────────────────────────────────────────────┐
│ SIGNATURE                                      │
└────────────────────────────────────────────────┘
```

#### SYNC
obtain `peer-list` from another peer
```
┌─────────┬─────────────────────────┬────────────┐
│ SYNC    │ Public Key              │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
└─────────┴──────────────────────────────────────┘
```
#### ADD
request to be added as a peer to another peers `peer-list` (only useful when hosting own data)
```
┌─────────┬─────────────────────────┬────────────┐
│ ADD     │ Public Key              │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
└─────────┴──────────────────────────────────────┘
```

#### REMOVE
remove this host from other's `peer-list`
```
┌─────────┬─────────────────────────┬────────────┐
│ REMOVE  │ Peer Address            │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
└─────────┴──────────────────────────────────────┘
```

#### PING
ping a peer to see if it is still alive and serving any data
```
┌─────────┬─────────────────────────┬────────────┐
│ PING    │ Peer Address            │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
└─────────┴──────────────────────────────────────┘
```

#### MIRROR
request and mirror a peers local `resources`
```
┌─────────┬─────────────────────────┬────────────┐
│ PING    │ Peer Address            │ YARN/1.0   │
├─────────┼─────────────────────────┴────────────┤
└─────────┴──────────────────────────────────────┘
```

### Request Headers

The headers below might be essential for some request types, but aren't essential to the inner working of the network.

## Response

### Response Types

### Response Headers

## Peers

Peers are essential to building a peer-to-peer network as they provide the avenues through which `resources` can be exchanged. To provide the capability of networking beyond direct connections between peers (0-Hop), those peers have to be aware of others on the network.

## Caching

## Signature and Attribution

Signatures form the foundation this protocol is built on. They identify and verify `resources`, and enable more advanced features such as **mutability**. A signature can be generated using a randomised seed, which is hashed using SHA256 and produces a secure private key. This key should not be shared, as those who possess it are able to issue requests to mutate the content associated with it.

```
┌────┐  ┌──────┐  ┌───────────┐
│SEED├──┤SHA256├─►│Private Key│
└────┘  └──────┘  └───────────┘
```

The raw public key is then extrapolated from the private key through the use of the secp256k elliptic curve and will be used as a unique identifier for the `resources` associated with it.

```
┌───────────┐  ┌────────┐  ┌──────────────┐
│Private Key├──┤secp256k├─►│Raw Public Key│
└───────────┘  └────────┘  └──────────────┘
```

Addressing and attribution in YARN is analogous to a simple filing cabinet: to locate a resource in a filing cabinet one can either search endlessly or know, where a certain resource is to be found; the latter is quite obviously a lot faster (though less serendipitous). Now what we can do now is not only ask our local filing cabinet management, if there's a certain `resource` available inside a folder with the label we are looking for; should that be the case we receive our `resource` otherwise the local management forwards our request to all the other cabinets in a certain region, thus increasing the chances of receiving a successful response.

Now we can either trust the managing authority, that what they have sent us is indeed valid information from the location we want, or we can attach some kind of information, only the original author can generate, but which we can verify - this is what we call **attribution** or the ability to verify the authenticity of a `resource`.
