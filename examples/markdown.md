# Huge header
###### Tiny header

Some text:
blah-blah-blah...

New paragraph. *Italic*. **Bold**. _alternative_ __style__.
++Underlined++. ~~Striked~~. `Mono`.

What about __*strange*__?
__*Even* stranger__?
_**Apocalyptically** strange_?
*__Different__ order*?

(Note: \*\*\* means bold and italic. In that exact order.
So closing in any over will produce incorrect HTML.
To make things even worse, no error will be produced.
Unclosed modificators are similar.)

----

This was horizontal rule.

> This is
> *quote*.

```
bool Block::is\_code()
{
    **return** this->should\_be() + 1;
}
```

![Awesome alt](../awseome.jpg "Awesome Title")

[Awesome link](example.com "Awesome title of link")

[![Magic alt](../magic.png "Ignored")](example.com "Magic title of linked pic")
