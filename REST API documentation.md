# Sticky-notes REST API documentation

This is the Sticky-notes REST API Reference. It provides API documentation, request/response samples, and usage description.

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

> The user must be able to
> - create notes with title, text and tags
> - edit/delete notes 
> - create/edit/delete tags
> - search for Notes: by title / by text / by tag

## REST API

| Method                | Endpoint                            | Description  |                             
| :------:              | ------                              | ------       |                               
| [GET](#Request)       | [/notes](#Request)                  | Lists all notes created by the user. |     
| [GET](#Request-1)     | [/notes/{noteId}](#Request-1)       | List information for a note. |
| [GET](#Request-2)     | [/notes/tags](#Request-2)           | Lists all tags created by the user and the asscoicated notes with them. |
| [GET](#Request-3)     | [/notes/tags/{tagId}](#Request-3)   | List information for a tag. |
| [GET](#Request-4)     | [/notes?title={string}&text={string}&tagName={string}&tagId={tagId}](#Request-4) | Filter notes: _by title_ and/or _by text_ and/or _by tags_ |
| [POST](#Request-5)    | [/notes](#Request-5)                | Add/Create new notes. |
| [PUT](#Request-6)     | [/notes](#Request-6)                | Edit existing notes. |
| [POST](#Request-7)    | [/notes/tags](#Request-7)           | Add new tags.  |
| [PUT](#Request-8)     | [/notes/tags/](#Request-8)          | Edit existing tags. |
| [DELETE](#Request-9)  | [/notes/{noteId}](#Request-9)       | Delete existing note. |
| [DELETE](#Request-10) | [/notes/tags/{tagId}](#Request-10)  | Delete existing tags.  |




### Request

```console
GET localhost:9066/api/v0/notes
```
Lists all notes created by the user.

### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of notes of type text/json. <br>
```json
`__**TODO**__` PASTE JSON RESPONSE
```

---

### Request

```console
GET localhost:9066/api/v0/notes/{noteId}
```
List information for a note. <br>
Path parameters <br>
_String `noteId` - Identifiers that notes must have to match the filter._

### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns string of type text/json. <br>
```json
`__**TODO**__` PASTE JSON RESPONSE
```

---

### Request

```console
GET localhost:9066/api/v0/notes/tags
```
Lists all tags created by the user and the asscoicated notes with them.


### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of tags of type text/json. <br>
```json
`__**TODO**__` PASTE JSON RESPONSE
```

---

### Request

```console
GET localhost:9066/api/v0/notes/tags/{tagId}
```
List information for a tag.

_TODO Add path param info.

### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of tags of type text/json. <br>
```json
`__**TODO**__` PASTE JSON RESPONSE
```


--- 

### Request

```console
GET /notes?title={string}&text={string}&tagName={string}&tagId={tagId}
```
Search for notes: _by title_ and/or _by text_ and/or _by tagName <br>
_TODO: 


### Reponse
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of notes of type text/json. <br>
```json
`__**TODO**__` PASTE JSON RESPONSE
```

--- 

### Request

```console
POST localhost:9066/api/v0/notes
```
Add/Create new notes. <br>
_TODO Add Request body_ <br>

### Reponse
204 No Content <br>
400 Bad Request <br>
201 Created <br>
Returns identifiers of the created notes of type text/json. <br>

```json
`__**TODO**__` PASTE JSON RESPONSE
```

---

### Request

```console
PUT localhost:9066/api/v0/notes
```
Edit existing notes. <br>
_TODO Add Request body - JSON Array of obj_ <br>

### Reponse
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 
400 Bad Request <br>

```json
`__**TODO**__` PASTE JSON RESPONSE
```

---

### Request

```console
POST localhost:9066/api/v0/notes/tags
```
Add new tags. <br>
_TODO Add Request body - JSON Array of obj_ <br>

### Reponse
400 Bad Request <br>
201 Created <br>
Returns identifiers of the created tags of type text/json. <br>

```json
`__**TODO**__` PASTE JSON RESPONSE
```

---

### Request

```console
PUT localhost:9066/api/v0/notes/tags
```
Edit existing tags. <br>
_TODO Add Request body - JSON Array of obj. Example tagId-> Tagname, TagAssociation_ <br>

### Reponse
400 Bad Request <br>
_TODO_ already exists status code
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

```json
`__**TODO**__` PASTE JSON RESPONSE
```

---

### Request

```console
DELETE localhost:9066/api/v0/notes/{noteId}
```
Delete existing note. <br>
_TODO Add path params <br>

### Reponse
400 Bad Request <br>
404 Not Found
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---

### Request

```console
DELETE localhsot:9066/api/v0/notes/tags/{tagId}
```
Delete existing tags. <br>
_TODO Add path param <br>

### Reponse
400 Bad Request <br>
404 Not Found
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---