# Sticky-notes REST API documentation

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

The business requierments that we need to met are: 

> The` user must be able to
> - create notes with title, text and tags .
> - edit notes
> - delete notes
> - search for Notes: by title / by text / by tags

## REST API

| Method | Endpoint | description| status codes |
| ------ | ------   | ------     | ------            |
| GET    | /notes   | Lists all notes created by the user.  | _todo_ |
| GET    | /notes/tags | Lists all tags created by the user and the asscoicated notes {noteId: title} with them. | _todo_ |
| GET    |  _todo_ /notes?searcByTtitle=$1&searcByText=$2&searcByTag=$3 searcByText maybe be long. Put In URL or send in body as json? | Search for notes: _by title_ and/or _by text_ and/or _by tags_ | 400 Bad Request, 404 Not Found, 200 OK returns string of type application/json  |
| POST    | /notes/:title | Add new note. Param: "title" | _todo_ |
