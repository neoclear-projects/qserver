# Never use cmake in important projects. It does not have detailed control over codes 

# Requests from client (Data Format)
## Send Name
First byte: 0x1
Following bytes: Name

## Send message
First byte: 0x0
Following bytes: Message

## Requesting past information
First byte: 0x2
Next 4 bytes: size of past information

## Get all latest messages (sync latest data)
First byte: 0x3
Next 4 bytes: year
Next 1 bytes: month
Next 1 bytes: day
Next 1 bytes: hour
Next 1 bytes: minute
Next 1 bytes: second


# Responses from server (Data Format)
## Reserved
First byte: 0x1

## Send message back
First byte: 0x0
Next 4 bytes: year
Next 1 bytes: month
Next 1 bytes: day
Next 1 bytes: hour
Next 1 bytes: minute
Next 1 bytes: second
Next 1024 bytes: sender's username
rest bytes: message

## Response of past information
First byte: 0x2
Next 4 bytes: year
Next 1 bytes: month
Next 1 bytes: day
Next 1 bytes: hour
Next 1 bytes: minute
Next 1 bytes: second
Next 1024 bytes: sender's username
rest bytes: message

## Response of sync information
First byte: 0x3
Next 4 bytes: year
Next 1 bytes: month
Next 1 bytes: day
Next 1 bytes: hour
Next 1 bytes: minute
Next 1 bytes: second
Next 1024 bytes: sender's username
rest bytes: message

