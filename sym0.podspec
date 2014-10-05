Pod::Spec.new do |s|
  s.name         = "sym0"
  s.version      = "1.5.1"
  s.source       = { :git => "https://github.com/mxcl/sym0.git" }
  s.requires_arc = true
  s.source_files = 'sym0.h'

  s.homepage = "https://github.com/mxcl/#{s.name}"
  s.source = { :git => "https://github.com/mxcl/#{s.name}.git", :tag => s.version }
  s.license = { :type => 'Public Domain', :text => 'Sup, CocoaPods linter?' }
  s.summary = 'Zero symbols, some functionality.'
  s.social_media_url = 'https://twitter.com/mxcl'
  s.authors  = { 'Max Howell' => 'mxcl@me.com' }
  s.source_files = 'sym0.h'
  s.requires_arc = true
end
