# CSV Generator

A very simple command line csv maker:

## Compile:
`make`

## Arguments:

```
Required:
-h specify headers ie foo bar baz
-d specify data types corresponding to header valid types: number string date
-rc specify row count i.e 100000

Optional:
-n file name, csv is suffixed by programme if not specified a uuid will be assigned
```

## Usage:
`./create_csv -h foo bar baz -d number string date -rc 5 -n example`

outputs: `example.csv`

```
foo,bar,baz
1,136f3176-94d3-4055-a5dc-ae18c3756448,2020-12-23
2,6d8bd94a-e438-40ec-b117-93f467a5b391,2020-12-26
3,aad45e91-059e-4fe8-ad82-644295ab5962,2021-01-11
4,d8e8a984-333c-4bf4-bf67-c4a5bd8c4f04,2021-01-04
5,4487ffe8-5f28-48eb-9f86-297e9a8fc83f,2021-01-05
```

## Data types
- `date` type generation will arbitrarily pick a date between: `2020-05-20` and `2020-12-23`
- `number` type will just be the line number
- `string` a uuid

