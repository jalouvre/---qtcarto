/***************************************************************************************************
**
** $QTCARTO_BEGIN_LICENSE:GPL3$
**
** Copyright (C) 2016 Fabrice Salvaire
** Contact: http://www.fabrice-salvaire.fr
**
** This file is part of the QtCarto library.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QTCARTO_END_LICENSE$
**
***************************************************************************************************/

/**************************************************************************************************/

#include "geoportail_elevation_reply.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/**************************************************************************************************/

QcGeoportailElevationReply::QcGeoportailElevationReply(QNetworkReply * reply,
                                                       const QVector<QcGeoCoordinateWGS84> & coordinates)
  : m_reply(reply),
    m_coordinates(coordinates)
{
  connect(m_reply, SIGNAL(finished()),
	  this, SLOT(network_reply_finished()));

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
	  this, SLOT(network_reply_error(QNetworkReply::NetworkError)));
}

QcGeoportailElevationReply::~QcGeoportailElevationReply()
{
  if (m_reply) {
    m_reply->deleteLater();
    m_reply = nullptr;
  }
}

/**************************************************************************************************/

void
QcGeoportailElevationReply::set_finished(bool finished)
{
  m_is_finished = finished;
  if (m_is_finished)
    emit this->finished();
}

void
QcGeoportailElevationReply::set_error(QcGeoportailElevationReply::Error error, const QString & error_string)
{
  m_error = error;
  m_error_string = error_string;
  emit this->error(error, error_string);
  set_finished(true);
}

/**************************************************************************************************/

void
QcGeoportailElevationReply::abort()
{
  if (m_reply)
    m_reply->abort();
}

QNetworkReply *
QcGeoportailElevationReply::network_reply() const
{
  return m_reply;
}

// Handle a successful request : store image data
void
QcGeoportailElevationReply::network_reply_finished()
{
  if (!m_reply)
    return;

  if (m_reply->error() != QNetworkReply::NoError) // Fixme: when ?
    return;

  QString ELEVATIONS = "elevations";
  QByteArray json_data = m_reply->readAll();
  // { "elevations": [ { "lon": 0.23, "lat": 48.05, "z": 112.73, "acc": 2.5}, ... ] }
  // qInfo() << json_data;
  QJsonDocument json_document(QJsonDocument::fromJson(json_data));
  const QJsonObject & json_root_object = json_document.object();
  if (json_root_object.contains(ELEVATIONS)) {
    QJsonArray json_array = json_root_object[ELEVATIONS].toArray();
    for (const auto & json_ref : json_array) {
      const QJsonObject & json_object = json_ref.toObject();
      double longitude = json_object["lon"].toDouble();
      double latitude = json_object["lat"].toDouble();
      double elevation = json_object["z"].toDouble();
      double elevation_accuracy = json_object["acc"].toDouble();
      m_elevations << QcGeoElevationCoordinateWGS84(longitude, latitude, elevation);
    }
  }
  qInfo() << m_elevations;

  // Fixme: duplicated code
  set_finished(true);
  m_reply->deleteLater();
  m_reply = nullptr;
}

// Handle an unsuccessful request : set error message
void
QcGeoportailElevationReply::network_reply_error(QNetworkReply::NetworkError error)
{
  if (!m_reply)
    return;

  if (error != QNetworkReply::OperationCanceledError)
    set_error(QcGeoportailElevationReply::CommunicationError, m_reply->errorString());

  set_finished(true);
  m_reply->deleteLater();
  m_reply = nullptr;
}

/**************************************************************************************************/

// #include "geoportail_elevation_reply.moc"

/***************************************************************************************************
 *
 * End
 *
 **************************************************************************************************/
