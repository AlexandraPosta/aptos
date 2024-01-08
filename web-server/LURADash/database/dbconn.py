'''
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database query functions and configuration
'''

from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

def query(cls, **kw):
    q = db.session.query(cls)
    if kw:
        q = q.filter_by(**kw)
    return q

def get(cls, id):
    return cls.query().get(id)

def exists(cls, **kw):
    return cls.query(**kw).first() is not None

def apply_kwargs(self, kwargs):
    for key, value in kwargs.items():
        setattr(self, key, value)
    return self

def setup_db_model():
    db.Model.query = classmethod(query)
    db.Model.get = classmethod(get)
    db.Model.exists = classmethod(exists)
    db.Model.apply_kwargs = apply_kwargs

db = SQLAlchemy(session_options=dict(expire_on_commit=False))
