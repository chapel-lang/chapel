#!/usr/bin/env ruby
#
# Simple(ish) Ruby Sinatra bot to verify that all commits on a Github
# pull request have a Signed-off-by line in the commit message.
#
# Note that this code was heavily inspired by
# http://git-scm.com/book/en/v2/GitHub-Scripting-GitHub
#
##############################################################################

require 'rubygems'
require 'httparty'
require 'sinatra'
require 'json'

# Put Sinatra on flatbed port 5000 (a fairly arbitrary choice)
set :port, 5000

# Globals
user_agent = 'ofiwg/signed-off-by-checker'

# Read the Github auth token in from the environment (it wouldn't do
# the hard-code it where it would show up in a public Github repo!)
auth_token = ENV['GITHUB_AUTH_TOKEN']
if auth_token == nil then
  puts "Someone forgot to set \$GITHUB_AUTH_TOKEN before launching me. Aborting!\n"
  exit 1
end

# Github webhooks are not delivered as HTTP GETs.  If we don't have
# this Sinatra method here, Sinatra displays a goofy error message if
# someone just visits "https://url_to_this_script".
get '/' do
  'Nothing to see here but us chickens'
end

# Github delivers webhooks via HTTP POSTs.
post '/' do
  push = JSON.parse(request.body.read)
  repo_name = push['repository']['full_name']

  # If this is not a push on a pull request (i.e., if there's no
  # commits to examine, such as if this is a test webhook ping from
  # github), then just return HTTP status 200 (i.e., success) with a
  # handy message that you can see in the Github webhook debug logs.
  if push['action'] == nil || (push['action'] != 'synchronize' &&
                               push['action'] != 'opened') then
    return [200, 'This is not a pull request opened or synchronize push; nothing for this bot to do!']
  end

  # This webhook will have only delivered the *new* commits on this
  # PR.  We need to examine *all* the commits -- so we can discard the
  # commits that were delivered in this webhook push.  Instead, do a
  # fetch to get all the commits on this PR (note: putting the
  # Authorization header in this request just in case this is a
  # private repo).
  commits_url = push['pull_request']['commits_url']
  commits = HTTParty.get(commits_url,
      :headers => {
        'Content-Type'  => 'application/json',
        'User-Agent'    => user_agent,
        'Authorization' => "token #{auth_token}" }
    )

  # Sanity check: If we've got no commits, do nothing.
  if commits.length == 0 then
    return [200, 'Somehow there are no commits on this PR... Weird...']
  end

  # Ok, we have commits.  Go through them.
  happy = true
  targetURL = 'https://github.com/ofiwg/libfabric/wiki#how-to-contribute'
  debug_message = "checking debug URL: #{commits_url}\n\n"
  final_message = ''

  commits.each_with_index do |commit, index|
    sha = commit['sha']
    status_url = "https://api.github.com/repos/#{repo_name}/statuses/#{sha}"
    debug_message += "examining commit index #{index} / sha #{sha}:\nstatus url: #{status_url}\n\n#{commit.to_json}\n\n"

    status = {
      'context' => 'Signed-off-by checker'
    }

    # Look for a Signed-off-by string in this commit
    if /Signed-off-by/.match commit['commit']['message']
      status['state']       = 'success'
      status['description'] = 'This commit is signed off'
    else
      status['state'] = 'failure'
      status['description'] = 'This commit is not signed off'
      status['target_url']  = targetURL

      happy = false
    end
    final_message = status['description']

    # If this is the last commit in the array (and there's more than
    # one commit in the array), override its state and description to
    # represent the entire PR (because Github shows the status of the
    # last commit as the status of the overall PR).
    if index == (commits.length - 1) && index > 0 then
      if happy then
        status['state']       = 'success'
        status['description'] = 'All commits were signed off.  Yay!'
      else
        status['state']       = 'failure'
        status['description'] = 'At least one commit was not signed off'
        status['target_url']  = targetURL
      end
      final_message         = status['description']
    end

    # Send the result back to Github for this specific commit
    HTTParty.post(status_url,
      :body => status.to_json,
      :headers => {
        'Content-Type'  => 'application/json',
        'User-Agent'    => user_agent,
        'Authorization' => "token #{auth_token}" }
    )
  end

  # Return HTTP status 200 (success) and a handy message that shows up
  # in the Github webhook debug logs.
  return [200, "#{debug_message} -- #{final_message}"]
end
