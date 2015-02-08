#!/usr/bin/env python

"""Portable email sender. Acts as replacement for mail, Mail, mailx,
email (cygwin). Message body is taken from stdin.
"""

from __future__ import print_function

try:
    import envelopes
except ImportError:
    import traceback
    traceback.print_exc()
    print()
    print('"envelopes" package is required. To install: pip install envelopes')
    print('Alternatively, use a mail client like: mail, Mail, email, etc')
    exit(1)

import getpass
import logging
import optparse
import socket
import sys


def main():
    """Parse command line arguments and send email!"""
    args = _parse_args()
    _setup_logging(args.verbose)
    body = sys.stdin.read()

    # Send the email!
    send_email(args.recipients, body, args.subject, args.header, args.sender, args.smtp_host)


def send_email(recipients, body, subject=None, headers=None, sender=None, smtp_host='localhost'):
    """Send email!

    :arg recipients: list of recipients. If only one, may be a string.
    :arg body: The email message body.
    :arg subject: Optional subject. Defaults to ''.
    :arg headers: Optional dict of headers to add.
    :arg sender: Optional sender address. Defaults to <user>@<fqdn>
    :arg smtp_host: Optional SMTP host. Defaults to 'localhost'.
    """
    if sender is None:
        sender = _default_sender()

    msg = envelopes.Envelope(
        to_addr=recipients,
        from_addr=sender,
        subject=subject,
        headers=headers,
        text_body=body
    )

    logging.info('Sending email via {0}: {1}'.format(smtp_host, msg))
    msg.send(smtp_host)


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
    """Return default sender address, which is <user>@<hostname>."""
    return '{0}@{1}'.format(getpass.getuser(), socket.getfqdn())


def _parse_args():
    """Parse and return command line arguments."""
    class NoWrapHelpFormatter(optparse.IndentedHelpFormatter):
        """Help formatter that does not wrap the description text."""

        def _format_text(self, text):
            return text

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
        default=_default_sender(),
        help='Sender email address.'
    )
    mail_group.add_option(
        '--smtp-host',
        default='localhost',
        help='SMTP host to use when sending email.'
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
