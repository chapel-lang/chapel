#!/usr/bin/env python3

"""Portable email sender. Acts as replacement for mail, Mail, mailx,
email (cygwin). Message body is taken from stdin.
"""

from __future__ import print_function

import email.mime.text
import getpass
import logging
import optparse
import os
import smtplib
import socket
import sys

try:
    basestring
except NameError:
    basestring = str

def main():
    """Parse command line arguments and send email!"""
    args = _parse_args()
    _setup_logging(args.verbose)
    body = sys.stdin.read()
    # Send the email!
    send_email(args.recipients, body, args.subject, args.header, args.sender, args.smtp_host,
        args.smtp_user, args.smtp_password, args.smtp_password_file)


def send_email(recipients, body, subject=None, headers=None, sender=None, smtp_host=None,
        smtp_user=None, smtp_password=None, smtp_password_file=None):
    """Send email!

    :arg recipients: list of recipients. If only one, may be a string.
    :arg body: The email message body.
    :arg subject: Optional subject. Defaults to ''.
    :arg headers: Optional dict of headers to add.
    :arg sender: Optional sender address. Defaults to <user>@<fqdn>
    :arg smtp_host: Optional SMTP host. Defaults to 'localhost'.
    :arg smtp_user: Optional SMTP user for login over TLS.
    :arg smtp_password: password for smtp_user, if used.
    :arg smtp_password_file: path to a file containing the password for smtp_user, if used.
    """
    if isinstance(recipients, basestring):
        recipients = [recipients]
    sender = sender or _default_sender()
    subject = subject or ''
    smtp_host = smtp_host or _default_smtp_host()

    msg = email.mime.text.MIMEText(body)

    msg['Subject'] = subject
    msg['From'] = sender
    msg['To'] = ','.join(recipients)

    if headers:
        for key, value in headers.items():
            msg[key] = value

    if not os.environ.get('CHPL_TEST_NOMAIL', ''):
        logging.debug('Opening connection to: {0}'.format(smtp_host))
        with smtplib.SMTP(smtp_host) as smtp:
            if smtp_user:
                logging.debug('Login with user={0}'.format(smtp_user))
                smtp.starttls()
                smtp.login(smtp_user,
                           _get_smtp_password(smtp_user, smtp_password,
                                              smtp_password_file))
            logging.info('Sending email to: {0} from: {1} subject: {2}'.format(
                ','.join(recipients), sender, subject))
            logging.debug('Email headers: {0}'.format(headers))
            logging.debug('Email body length: {0}'.format(len(body)))
            smtp.sendmail(sender, recipients, msg.as_string())
    else:
        logging.info('CHPL_TEST_NOMAIL: no email to: {0} from: {1} subject: {2}'.format(
            ','.join(recipients), sender, subject))


def _parse_headers(option, opt, value, parser, *args, **kwargs):
    """OptionParser callback function for parsing header values passed by user.

    It takes values that have commas (e.g. the user specified
    --header=Reply-To=X@y.com,Precedence=bulk), breaks them apart and adds the
    individual name/value pairs to the dict of values.
    """
    # Get the existing values the parser knows about for this particular
    # option.
    value_dict = getattr(parser.values, option.dest, None) or {}

    # Split the value provided.
    parsed_vals = value.split(',')
    for v in parsed_vals:
        key, value = v.split('=')
        value_dict[key] = value

    # Set the updated dict to the oiption value.
    setattr(parser.values, option.dest, value_dict)


def _default_sender():
    """Return default sender address, which is <user>@<hostname> unless
    CHPL_UTIL_SMTP_SENDER is set in environment.
    """
    return os.environ.get('CHPL_UTIL_SMTP_SENDER', '{0}@{1}'.format(getpass.getuser(), socket.getfqdn()))

def _default_smtp_host():
    """Return default smtp host, which is localhost unless CHPL_UTIL_SMTP_HOST is
    set in environment.
    """
    return os.environ.get('CHPL_UTIL_SMTP_HOST', 'localhost')

def _default_smtp_user():
    """Return default smtp user from environment variable CHPL_UTIL_SMTP_USER.
    """
    return os.environ.get('CHPL_UTIL_SMTP_USER')

def _default_smtp_password():
    """Return default password for smtp user from environment variable CHPL_UTIL_SMTP_PASSWORD.
    """
    return os.environ.get('CHPL_UTIL_SMTP_PASSWORD')

def _default_smtp_password_file():
    """Return default password file for smtp user from env var CHPL_UTIL_SMTP_PASSWORD_FILE.
    """
    password_file = os.environ.get('CHPL_UTIL_SMTP_PASSWORD_FILE')
    if not password_file:
        password_file = os.path.expanduser(os.path.join('~', '.ssh', 'chpl_util_smtp_password.txt'))
    return password_file

def _get_smtp_password(smtp_user=None, smtp_password=None, smtp_password_file=None):
    """Return the smtp password based on available info.

    :arg smtp_user: smtp_user to login to smtp_host
    :arg smtp_password: password for smtp_user
    :arg smtp_password_file: path to a file containing the password for smtp_user
    """

    if smtp_password:
        logging.debug('Using --smtp-password')
        return smtp_password

    password = None
    if smtp_password_file:
        logging.debug('Reading --smtp-password-file={0}'.format(smtp_password_file))
        if os.path.isfile(smtp_password_file):
            with open(smtp_password_file, 'r') as fp:
                password = fp.readline(80)
        else:
            raise ValueError('--smtp-password-file={0} not found'.format(smtp_password_file))

    if password:
        logging.debug('Using password from file')
        return password.strip()
    else:
        raise ValueError('--smtp-user={0} requires valid --smtp-password or --smtp-password-file'.format(smtp_user))

def _parse_args():
    """Parse and return command line arguments."""
    class NoWrapHelpFormatter(optparse.IndentedHelpFormatter):
        """Help formatter that does not wrap the description text."""

        def _format_text(self, text):
            return text

    show_default_text = lambda value: ' (default: %default)' if value else ''
    hide_default_text = lambda value: ' (default: ********)' if value else ''

    parser = optparse.OptionParser(
        usage='usage: %prog [options] recipient_email [...]',
        description=__doc__,
        formatter=NoWrapHelpFormatter()
    )

    parser.add_option(
        '-v', '--verbose',
        action='store_true',
        help='Verbose output.'
    )

    mail_group = optparse.OptionGroup(parser, 'Mail Options')

    mail_group.add_option(
        '-s', '--subject',
        default=None,
        help='Email subject.'
    )
    mail_group.add_option(
        '-H', '--header',
        action='callback', type='string',
        callback=_parse_headers,
        help=('Email header(s) of form NAME=VALUE. '
              'Specify more than one with comma delimited list.')
    )
    mail_group.add_option(
        '-S', '--sender',
        metavar='CHPL_UTIL_SMTP_SENDER',
        default=_default_sender(),
        help='Sender email address. (default: %default)'
    )
    mail_group.add_option(
        '--smtp-host',
        metavar='CHPL_UTIL_SMTP_HOST',
        default=_default_smtp_host(),
        help='SMTP host to use when sending email. (default: %default)'
    )
    mail_group.add_option(
        '-u', '--smtp-user', metavar='CHPL_UTIL_SMTP_USER',
        default=_default_smtp_user(),
        help='Optional user for SMTP login. Also requests TLS connection.' +
            show_default_text(_default_smtp_user())
    )
    mail_group.add_option(
        '-p', '--smtp-password', metavar='CHPL_UTIL_SMTP_PASSWORD',
        default=_default_smtp_password(),
        help='Password for --smtp-user.' +
            hide_default_text(_default_smtp_password())
    )
    mail_group.add_option(
        '-P', '--smtp-password-file', metavar='CHPL_UTIL_SMTP_PASSWORD_FILE',
        default=_default_smtp_password_file(),
        help='Path to a file containing the password for --smtp-user.' +
            show_default_text(_default_smtp_password_file())
    )

    parser.add_option_group(mail_group)

    opts, args = parser.parse_args()

    # Add all positional arguments as recipients.
    opts.recipients = args
    return opts


def _setup_logging(verbose=False):
    """Initialize logging and set level based on verbose.

    :type verbose: bool
    :arg verbose: When True, set log level to DEBUG.
    """
    log_level = logging.DEBUG if verbose else logging.WARN
    logging.basicConfig(format='%(asctime)s [%(levelname)s] %(message)s',
                        level=log_level)
    logging.debug('Verbose output enabled.')


if __name__ == '__main__':
    main()
