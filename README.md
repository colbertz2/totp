# TOTP Demo Application
Demo implementation of RFC 6238 protocol for time-based one-time password.

## Usage
```shell
$ totp <command> [<args>]
```

### Commands
```
keygen    generates a new private key
qrgen     generates a client registration QR code (JPG)
code      returns the current 6-digit TOTP
test      compare given TOTP to expected TOTP at the current time
```