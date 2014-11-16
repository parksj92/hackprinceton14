json.array!(@activities) do |activity|
  json.extract! activity, :id, :date, :activity_type, :duration
  json.url activity_url(activity, format: :json)
end
