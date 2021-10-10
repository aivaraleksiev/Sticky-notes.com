# Sticky-notes REST API documentation

This is the Sticky-notes REST API Reference. It provides API documentation, request/response samples, and usage description.

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

> The` user must be able to
> - create notes with title, text and tags
> - edit/delete notes 
> - create/edit/delete tags
> - search for Notes: by title / by text / by tag

## REST API

| Method               | Endpoint                       | description  |                             
| :------:             | ------                         | ------       |                               
| [GET](#Request)      | [/notes](#Request)             | Lists all notes created by the user. |     
| [GET](#Request-1)    | [/notes/{noteId}](#Request-1)  | List information for a note with 'noteId'. |
| [GET](#Request-2)    | [/notes/tags](#Request-2)      | Lists all tags created by the user and the asscoicated notes with them. |
| [GET](#Request-3)    | [/notes?byTtitle=$1&byText=$2&byTagName=$3&byTagId=$4](#Request-3) `TODO` Use requestBody for `searchByText` - too long. OR JUST /notes and request body?? | Filter notes: _by title_ and/or _by text_ and/or _by tags_ `TODO` Can I merge it with the first method |
| [POST](#Request-4)   | [/notes](#Request-4)           | Add/Create new notes. |
| [PUT](#Request-5)    | [/notes](#Request-5)           | Update/Edit existing notes. |
| [POST](#Request-6)   | [/notes/tags](#Request-6)      | Add new tags.  |
| [PUT](#Request-7)    | [/notes/tags/](#Request-7)     | Edit existing tags'  name and associations. |
| [DELETE](#Request-8) | [/notes/](#Request-8)          | Delete existing notes. |
| [DELETE](#Request-9) | [/notes/tags/](#Request-9)     | Delete existing tags.  |

Questions:
`_*TODO*_` Maybe add method to delete all notes/tags with one call.

## REST API another layout


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
GET /notes?searchByTtitle=$1&searchByText=$2&searchByTagName=$3&searchByTagId=$4 OR JUST /notes and request body??
```
Search for notes: _by title_ and/or _by text_ and/or _by tagName <br>
_TODO: QUESTION_ Use query params OR Request body? Should I use both `searchByTagName` and `searchByTagId`  or create `GET .../notes/tags/{tagsId} 


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
Update/Edit existing note. <br>
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
Edit existing tags' name and associations. <br>
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
DELETE localhost:9066/api/v0/notes
```
Delete existing notes. <br>
_TODO Add Request body - JSON Array._ <br>

### Reponse
400 Bad Request <br>
404 Not Found
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---

### Request

```console
DELETE localhsot:9066/api/v0/notes/tags
```
Delete existing tags. <br>
_TODO Add Request body - JSON Array._ <br>

### Reponse
400 Bad Request <br>
404 Not Found
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---