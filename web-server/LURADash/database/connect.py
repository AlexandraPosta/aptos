"""
	Leeds University Rocketry Organisation - LURA
    Author Name: A. Posta
    Description: MYSQL Database query functions and configuration
"""

from flask_sqlalchemy import SQLAlchemy


db = SQLAlchemy()


def query(cls, **kw):
    """Dababase query function

    Retrives all instances of a given class from the database.
    Filters them based on the keyword arguments.

    Args:
        cls: Class to query
        kw: Keyword arguments to filter the query

    Returns:
        Query object
    """
    q = db.session.query(cls)
    if kw:
        q = q.filter_by(**kw)
    return q


def get(cls, cls_id):
    """Dababase query function

    Retrives all instances of a given class from the database.
    Filters them based on the keyword arguments.

    Args:
        cls: Class to query
        kw: Keyword arguments to filter the query

    Returns:
        Query object
    """
    return cls.query().get(cls_id)


def exists(cls, **kw):
    """Dababase query function

    Check if table exists in the database.

    Args:
        cls: Class to check
        kw: Keyword arguments to filter the query

    Returns:
        Query object
    """
    return cls.query(**kw).first() is not None


def setup_db_model():
    """Setup database model

    Maps the custom query functions to the SQLAlchemy
    db session.

    Returns:
        None
    """
    db.Model.query = classmethod(query)
    db.Model.get = classmethod(get)
    db.Model.exists = classmethod(exists)

db = SQLAlchemy(session_options=dict(expire_on_commit=False))
