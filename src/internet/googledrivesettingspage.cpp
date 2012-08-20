/* This file is part of Clementine.
   Copyright 2012, David Sansome <me@davidsansome.com>
   
   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "googledriveclient.h"
#include "googledriveservice.h"
#include "googledrivesettingspage.h"
#include "ui_googledrivesettingspage.h"
#include "core/application.h"
#include "internet/internetmodel.h"
#include "ui/settingsdialog.h"

#include <QSortFilterProxyModel>

GoogleDriveSettingsPage::GoogleDriveSettingsPage(SettingsDialog* parent)
  : SettingsPage(parent),
    ui_(new Ui::GoogleDriveSettingsPage),
    service_(dialog()->app()->internet_model()->Service<GoogleDriveService>())
{
  ui_->setupUi(this);
  ui_->login_state->AddCredentialGroup(ui_->login_container);

  connect(ui_->login_button, SIGNAL(clicked()), SLOT(LoginClicked()));
  connect(ui_->login_state, SIGNAL(LogoutClicked()), SLOT(LogoutClicked()));
  connect(service_, SIGNAL(Connected()), SLOT(Connected()));
}

GoogleDriveSettingsPage::~GoogleDriveSettingsPage() {
  delete ui_;
}

void GoogleDriveSettingsPage::Load() {
  QSettings s;
  s.beginGroup(GoogleDriveService::kSettingsGroup);

  const QString user_email = s.value("user_email").toString();

  if (!user_email.isEmpty()) {
    ui_->login_state->SetLoggedIn(LoginStateWidget::LoggedIn, user_email);
  }
}

void GoogleDriveSettingsPage::Save() {
  QSettings s;
  s.beginGroup(GoogleDriveService::kSettingsGroup);
}

void GoogleDriveSettingsPage::LoginClicked() {
  service_->Connect();
}

void GoogleDriveSettingsPage::LogoutClicked() {
  service_->ForgetCredentials();
  ui_->login_state->SetLoggedIn(LoginStateWidget::LoggedOut);
}

void GoogleDriveSettingsPage::Connected() {
  QSettings s;
  s.beginGroup(GoogleDriveService::kSettingsGroup);

  const QString user_email = s.value("user_email").toString();

  ui_->login_state->SetLoggedIn(LoginStateWidget::LoggedIn, user_email);
}