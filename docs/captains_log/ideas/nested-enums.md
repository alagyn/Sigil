## Nested enums
```
enum ASDF
{
  A,
  B,
  enum Inner
  {
     C,
     D
  }
}

ASDF.A
ASDF.B
ASDF.Inner.C

```