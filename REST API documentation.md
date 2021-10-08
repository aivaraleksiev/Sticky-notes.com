# Sticky-notes REST API documentation

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

The business requierments that we need to met are: 

> The` user must be able to
> - create notes with title, text and tags .
> - edit notes
> - delete notes
> - search for Notes: by title / by text / by tags

## REST API

| Method | Endpoint | description| error status code |
| ------ | ------   | ------     | ------            |
| GET    | /notes   | Lists all notes created by the user.  | _todo_ |
| GET    | /notes/tags | Lists all tags created by the user and the asscoicated notes {noteId: Title} with them. | _todo_ |
| GET    |  _todo_ /notes?searcByTtitle=$1&searcByText=$2&searcByTag=$3  In URL or send in body as json? | Search for notes: _by title_ and/or _by text_ and/or _by tags_ | _todo_ |
